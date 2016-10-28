#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QMap>
#include <QColor>

class QFile;

class Options : public QObject
{
    Q_OBJECT
    public:
		enum Difficulte : quint8 {
			FACILE = 'f',
			MOYEN = 'm',
			DIFFICILE = 'd',
			PERSONNALISE = 'p'
		};

		enum OptionsFlags : quint8 {
			PT_INTERROGATION = 1 << 0,
			DECOUVERTE = 1 << 1,
			MODE_CLIC_DOUBLE = 1 << 2,
			ALL =  0b111
		};

		static Options* getInstance(QObject *parent = 0);
		static void deleteInstance();


        //getters
		Difficulte getDifficulte() const;
        qint8 getHauteur() const;
        qint8 getLargeur() const;
        qint16 getNbMines() const;
		bool isOptionEnabled(OptionsFlags option) const;
		QColor getColor(const QString element, const QColor defaultValue = Qt::white) const;
        //setters
		void setDifficulte(const Difficulte d, const qint8 h = 9, const qint8 l = 9, const qint16 m = 9);
		void setOptionFlag(const OptionsFlags option, const bool enabled);
		void setColor(const QString element, const QColor color);

        //IOFile
        void ecrire();
        void lire();

    signals:
        void changeDifficulte();

	public slots:
		void loadDefaultUI();

    private:
		explicit Options(QObject *parent = 0);
		~Options();

		static Options *This;
		
		QMap<QString,QColor> colors;
		QFile *file;

		struct ModeDifficulte {
			qint16 nbMines;
			qint8 hauteur;
			qint8 largeur;
			Difficulte difficulte;
		} difficulte;
        
		quint8 flags;
};

#endif // OPTIONS_H
