#ifndef STABLORASPONA_H
#define STABLORASPONA_H

#include <vector>
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include "strukture.h"

using namespace std;
/************************************************************************/
/* Klasa stablo raspona, napravljena prema algoritmu opisanom u 
   magistarskom radu, pomaže u pretrazi prozora pretrage, ispituje da li 
   krajnje taèke duži leže u prozoru pretrage						    */
/************************************************************************/
class StabloRaspona
{
private:
	struct Cvor
	{
	public:
		typedef boost::shared_ptr<Cvor> pCvor;

		pCvor lijevo_dijete;
		pCvor desno_dijete;
		pCvor p_korijen_sljedece_dim;

		KrajnjaTackaDuzi vrijednost;
	};

public:
	typedef boost::shared_ptr<Cvor> pCvor;

private:
	QVector<KrajnjaTackaDuzi> rezultat;
	int m_dimenzija;
	pCvor m_korijen;

	pCvor sortiranjeTacaka(QVector<KrajnjaTackaDuzi> tacke, int dimenzija);
	pCvor napraviStablo(QVector<KrajnjaTackaDuzi> tacke, int dimenzija);
	void podijeliNiz(QVector<KrajnjaTackaDuzi> tacke, QVector<KrajnjaTackaDuzi>& lijeve_tacke, QVector<KrajnjaTackaDuzi>& desne_tacke);
	pCvor razdvajajuciCvor(pCvor korijen, int pocetak, int kraj, int dimenzija);
	void jednoDimenzionalnaPretragaRaspona(pCvor korjen, int x1, int x2);
	void prijaviPodstablo(pCvor cvor);
	bool jelList(pCvor cvor);
	void dvodimenzionalnaPretragaRaspona(pCvor korjen, int x1, int x2, int y1, int y2);
public:
	StabloRaspona();
	StabloRaspona(QVector<KrajnjaTackaDuzi> tacke, int dimenzija);
	void pretragaRaspona(int x1, int x2, int y1, int y2);
	QVector<KrajnjaTackaDuzi> dajRezultat(){ return rezultat; }

};

#endif //STABLORASPONA_H
