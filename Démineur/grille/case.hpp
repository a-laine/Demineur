#ifndef CASE_HPP
#define CASE_HPP

#include <QGraphicsItemGroup>
#include <QHash>

class QPixmap;

class Case : public QGraphicsItemGroup
{
	public:
		enum TypeCase {
			INDEFINIE,
			DECOUVERTE,
			NON_DECOUVERTE,
			DRAPEAU,
			PT_INTERROGATION
		};

		static constexpr int size()
		{
			return 36;
		}

		explicit Case(QGraphicsScene *scene);
		explicit Case(QGraphicsScene *scene, TypeCase type, bool isMine = false);
		virtual ~Case();

		void updateColor();
		void setType(TypeCase type);
		void setValue(qint8 val);
		void setMine(bool isMine);
		TypeCase getType() const;
		qint8 getValue() const;
		bool isMine() const;
		virtual QRectF boundingRect() const;

	protected:
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *);

	private:
		static QHash<qint8,QPixmap*> pixmaps;
		static QPixmap* getPixmap(quint8 val);
	
		quint8 valeur;
};

#endif // CASE_HPP
