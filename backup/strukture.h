#ifndef STRUKTURE_H
#define STRUKTURE_H
#include <QPoint>
#include <QVector>

/************************************************************************/
/* @brief Struktura koja prezentuje duz */
/************************************************************************/
struct Duz
{
private:
    QPoint m_startnaKoordinata;
    QPoint m_krajnjaKoordinata;
    bool oznacena;
    int m_IDDuzi;

public:
    int vratiIdDuzi(){ return m_IDDuzi; }

    void postaviIDDuzi(int idDuzi)
	{ m_IDDuzi = idDuzi; }

    bool jelOznacena()
	{ return oznacena; }

    void oznaciDuz()
	{ oznacena = true; }

    void ponistiOznacenuDuz()
	{ oznacena = false; }

     int vratiPocetnuKoordinatu(int d)
	 {
		 switch(d)
		 {
        case 1: 
			return m_startnaKoordinata.x(); 
			break;
		case 2:
			return m_startnaKoordinata.y();
			break;
		default:
			return -1;
		 }
	 }

	 QPoint vratiPocetnuKoordinatu()
	 { return m_startnaKoordinata; }

     QPoint vratiKrajnjuKoordinatu()
	 { return m_krajnjaKoordinata; }

	 int vratiKrajnjuKoordinatu(int d)
	 {
		 switch(d)
		 {
		 case 1: 
			 return m_krajnjaKoordinata.x(); 
			 break;
		 case 2:
			 return m_krajnjaKoordinata.y();
			 break;
		 default:
			 return -1;
		 }
	 }

     void postaviPocetnuKoordinatu(QPoint pocetnaKoord)
	 { m_startnaKoordinata = pocetnaKoord; }

     void postaviKrajnjuKoordinatu(QPoint krajnjaKoord)
	 { m_krajnjaKoordinata = krajnjaKoord; }

};

/************************************************************************/
/* Struktura koja predstavlja krajnje taèke duži u ravni */
/************************************************************************/
struct KrajnjaTackaDuzi
{

private:
    QPoint m_koordinata;
    int m_idDuzi;

public:

    int vratiIdDuzi()
	{ return m_idDuzi; }

    int vratiKoordinatu(int d)
	{
		switch (d)
		{
			case 1: 
				return m_koordinata.x(); 
				break;
			case 2:
				return m_koordinata.y();
				break;
			default:
				return -1;
		}
    }

    void postaviKoordinatu(QPoint koordinata)
	{ m_koordinata = koordinata; }

    void postaviIdDuzi(int idDuzi)
	{ m_idDuzi = idDuzi; }

};


/************************************************************************/
/* Struktura koja opisuje elementarni interval, potrebna za stablo Duzi */
/************************************************************************/
struct ElementarniInterval
{

    int pocetak;
    int kraj;

	void postaviInterval(int x, int y)
	{ pocetak = x; kraj = y; } 

};

/************************************************************************/
/* @brief Struktura koju koristi sort da se definise 
    rastuci poredak po x-koordinati krajnjih taèaka duzi                */
/************************************************************************/
struct PoredjenjePoX
{
	
	bool operator()(const KrajnjaTackaDuzi& t1, const KrajnjaTackaDuzi& t2)
	{
		KrajnjaTackaDuzi tacka1, tacka2;
		tacka1 = t1;
		tacka2 = t2;
		return ((tacka1.vratiKoordinatu(1) < tacka2.vratiKoordinatu(1)) || 
			((tacka1.vratiKoordinatu(1) == tacka2.vratiKoordinatu(1)) && (tacka1.vratiKoordinatu(2) < tacka2.vratiKoordinatu(2))));
	}
};

/************************************************************************/
/* @brief Struktura koju koristi sort da se definise 
    rastuci poredak po y-koordinati krajnjih taèaka duzi                */
/************************************************************************/
struct PoredjenjePoY
{
	bool operator()(const KrajnjaTackaDuzi& t1, const KrajnjaTackaDuzi& t2)
	{
		KrajnjaTackaDuzi tacka1, tacka2;
		tacka1 = t1;
		tacka2 = t2;
		return ((tacka1.vratiKoordinatu(2) < tacka2.vratiKoordinatu(2)) || 
			((tacka1.vratiKoordinatu(2) == tacka2.vratiKoordinatu(2)) && (tacka1.vratiKoordinatu(1) < tacka2.vratiKoordinatu(1))));
	}
};

#endif // STRUKTURE_H
