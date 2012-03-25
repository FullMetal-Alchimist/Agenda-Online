#ifndef DEVOIR_HPP
#define DEVOIR_HPP

#include <QtCore>

class Devoir
{
    public:
        static void initSystem();
        Devoir();
        Devoir(quint64 id, QString name, QString matiere, QString sujet, QString classe, QDate date);

        quint64 getID() const
        {
            return id;
        }
        QString getClasse() const
        {
            return classe;
        }
        QString getName() const
        {
            return name;
        }
        QString getMatiere() const
        {
            return matiere;
        }
        QString getSujet() const
        {
            return sujet;
        }
        QDate getDate() const
        {
            return date;
        }
        static QPair<int, QChar> decodeClasse(QString const& classe)
        {
            if(classe.size() > 2)
                return QPair<int, QChar>();

            QPair<int, QChar> decoded;

            decoded.first = classe.at(0).digitValue();
            decoded.second = classe.at(1);

            return decoded;
        }
        static QString calculateLevel(int niveau)
        {
            if(niveau == 7)
                return QObject::tr("CM2");
            else if(niveau == 6)
                return QObject::tr("6ème");
            else if(niveau == 5)
                return QObject::tr("5ème");
            else if(niveau == 4)
                return QObject::tr("4ème");
            else if(niveau == 3)
                return QObject::tr("3ème");
            else if(niveau == 2)
                return QObject::tr("2nde");
            else if(niveau == 1)
                return QObject::tr("1ère");
            else if(niveau == -1)
                return QObject::tr("Terminale");
            else
                return QObject::tr("Inconnu");
        }

        friend QDataStream& operator>>(QDataStream& in, Devoir& devoir);
        friend QDataStream& operator<<(QDataStream& out, const Devoir& devoir);

    private:
        QString name;
        QString matiere;
        QString sujet;
        QString classe;

        QDate date;
        quint64 id;

};

Q_DECLARE_METATYPE(Devoir)

#endif // DEVOIR_HPP
