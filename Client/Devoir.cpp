#include "Devoir.hpp"

Devoir::Devoir()
{

}
Devoir::Devoir(quint64 id, QString name, QString matiere, QString sujet, QString classe, QDate date) : name(name), matiere(matiere), sujet(sujet), id(id), classe(classe), date(date)
{
}
QDataStream& operator<<(QDataStream& out, const Devoir& devoir)
{
    out << devoir.id << devoir.name << devoir.matiere << devoir.sujet << devoir.classe << devoir.date;
    return out;
}
QDataStream& operator>>(QDataStream& in, Devoir& devoir)
{
    in >> devoir.id >> devoir.name >> devoir.matiere >> devoir.sujet >> devoir.classe >> devoir.date;
    return in;
}
void Devoir::initSystem()
{
    qRegisterMetaType<Devoir>("Devoir");
    qRegisterMetaTypeStreamOperators<Devoir>("Devoir");
}

bool Devoir::operator ==(Devoir const& t)
{
    return id == t.id;
}
bool operator ==(Devoir const& a, Devoir const& b)
{
    return a == b;
}

