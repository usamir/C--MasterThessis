#include "stablo_raspona.h"

StabloRaspona::StabloRaspona(QVector<KrajnjaTackaDuzi> tacke, int dimenzija)
{
	this->m_dimenzija = dimenzija;
	m_korijen = sortiranjeTacaka(tacke, 1);
}

StabloRaspona::StabloRaspona() : m_dimenzija(0) {}


void StabloRaspona::podijeliNiz(QVector<KrajnjaTackaDuzi> tacke, QVector<KrajnjaTackaDuzi>& lijeve_tacke, QVector<KrajnjaTackaDuzi>& desne_tacke)
{
	int n = tacke.size();
	int n_lijevo = n - n / 2;

	int index(0);
	BOOST_FOREACH(KrajnjaTackaDuzi t, tacke)
	{
		if(index < n_lijevo)
		{
			lijeve_tacke.push_back(t);
		}
		else
		{
			desne_tacke.push_back(t);
		}
		index++;
	}
}


StabloRaspona::Cvor::pCvor StabloRaspona::napraviStablo(QVector<KrajnjaTackaDuzi> tacke, int dimenzija)
{
	Cvor::pCvor cvor(new Cvor);
	int n = tacke.size();

	if(n > 1)
	{
		QVector<KrajnjaTackaDuzi> lijeve_tacke;
		QVector<KrajnjaTackaDuzi> desne_tacke;
		podijeliNiz(tacke, lijeve_tacke,  desne_tacke);
		cvor->lijevo_dijete = napraviStablo(lijeve_tacke, dimenzija);
		cvor->desno_dijete = napraviStablo(desne_tacke, dimenzija);
		cvor->vrijednost = tacke.at(lijeve_tacke.size());
	}
	else
	{
		// Ako je list u pitanju, u vrijednost lista stavi vrijednost tacke
		cvor->vrijednost = tacke.at(0);
	}

	// Idi na sljedecu dimenziju, sortiraj tacke zavisnosti od dimenzije
	if(dimenzija + 1 <= this->m_dimenzija)
	{
		cvor->p_korijen_sljedece_dim = sortiranjeTacaka(tacke, dimenzija + 1);
	}
	return cvor;
}

StabloRaspona::Cvor::pCvor StabloRaspona::sortiranjeTacaka(QVector<KrajnjaTackaDuzi> tacke,
	int dimenzija)
{
	if (dimenzija == 1)
	{
		sort(tacke.begin(), tacke.end(), PoredjenjePoX());
	}
	else if (dimenzija == 2)
	{
		sort(tacke.begin(), tacke.end(), PoredjenjePoY());
	}
	
	return napraviStablo(tacke, dimenzija);
}

StabloRaspona::Cvor::pCvor StabloRaspona::razdvajajuciCvor(Cvor::pCvor korijen,
	int pocetak, int kraj, int koord)
{
	Cvor::pCvor v(korijen);
	while(!jelList(v) && (v->vrijednost.vratiKoordinatu(koord) >= kraj  ||
		v->vrijednost.vratiKoordinatu(koord) <= pocetak))
	{
		if(v->vrijednost.vratiKoordinatu(koord) >= kraj)
		{
			v = v->lijevo_dijete;
		}
		else
		{
			v = v->desno_dijete;
		}
	}
	return v;
}

void StabloRaspona::pretragaRaspona(KrajnjaTackaDuzi pocetak, KrajnjaTackaDuzi kraj, int dim)
{
	rezultat.clear();
	pretragaRaspona(m_korijen, pocetak, kraj, 1);
}

void StabloRaspona::prijaviPodstablo(Cvor::pCvor cvor)
{
	if(jelList(cvor))
	{
		rezultat.push_back(cvor->vrijednost);	
	}
	else
	{
		prijaviPodstablo(cvor->lijevo_dijete);	
		prijaviPodstablo(cvor->desno_dijete);
	}
}


bool StabloRaspona::jelList(Cvor::pCvor v)
{
	if (v->lijevo_dijete == NULL && v->desno_dijete == NULL)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

void StabloRaspona::pretragaRaspona(Cvor::pCvor korijen, KrajnjaTackaDuzi pocetak,
	KrajnjaTackaDuzi kraj, int dim)
{
	int start = pocetak.vratiKoordinatu(dim);
	int end = kraj.vratiKoordinatu(dim);

	Cvor::pCvor cvor_grananja = razdvajajuciCvor(korijen, start, end, dim);
	Cvor::pCvor v;

	// Ispitujem da li ako je cvor grananja list da li tacka u tom cvoru lezi u rasponu pretrage
	if (jelList(cvor_grananja))
	{
		int d = this->m_dimenzija;
		while (d > 0)
		{
			// Ispitujem da li vrijednost u svakoj koordinati tacke lezi u zadanom rasponu
			if(cvor_grananja->vrijednost.vratiKoordinatu(d) >= pocetak.vratiKoordinatu(d) 
				&& cvor_grananja->vrijednost.vratiKoordinatu(d) <= kraj.vratiKoordinatu(d))
			{	
				d--;
			}
			else
			{
				break;
			}
			if (d == 0)
			{
				rezultat.push_back(cvor_grananja->vrijednost);
			}	
		}
		
	} 
	else
	{
		// Dalje pretraga ide kao po algoritmu
		v = cvor_grananja->lijevo_dijete;
		while(!jelList(v))
		{
			if(start <= v->vrijednost.vratiKoordinatu(dim))
			{
				if (dim == this->m_dimenzija)
				{
					prijaviPodstablo(v->desno_dijete);
					v = v->lijevo_dijete;
				}
				else
				{
					pretragaRaspona(v->desno_dijete->p_korijen_sljedece_dim, pocetak, kraj, dim + 1);
					v = v->lijevo_dijete;
				}
				
			}
			else
			{
				v = v->desno_dijete;
			}
		}

		int d = this->m_dimenzija;
		while (d > 0)
		{
			if(v->vrijednost.vratiKoordinatu(d) >= pocetak.vratiKoordinatu(d) &&
				v->vrijednost.vratiKoordinatu(d) <= kraj.vratiKoordinatu(d))
			{	
				d--;
			}
			else
			{
				break;
			}
			if (d == 0)
			{
				rezultat.push_back(v->vrijednost);
			}
		}
		
		v = cvor_grananja->desno_dijete;
		while(!jelList(v))
		{
			if(v->vrijednost.vratiKoordinatu(dim) <= end)
			{
				if (dim == this->m_dimenzija)
				{
					prijaviPodstablo(v->lijevo_dijete);
					v = v->desno_dijete;
				}
				else
				{
					pretragaRaspona(v->lijevo_dijete->p_korijen_sljedece_dim, pocetak, kraj, dim + 1);
					v = v->desno_dijete;
				}
			}
			else
			{
				v = v->lijevo_dijete;
			}
		}


		d = this->m_dimenzija;
		while (d > 0)
		{
			if(v->vrijednost.vratiKoordinatu(d) >= pocetak.vratiKoordinatu(d) 
				&& v->vrijednost.vratiKoordinatu(d) <= kraj.vratiKoordinatu(d))
			{	
				d--;
			}
			else
			{
				break;
			}	
			
			if (d == 0)
			{
				rezultat.push_back(v->vrijednost);
			}
			
		}
		
	}	
}