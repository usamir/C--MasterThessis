#include "stablo_raspona.h"


  /**
  @ brief Konstruktor Stabla Raspona

  @param [in] tacke vektor kranjih taèaka duži koje æemo pretraziti
  @param [in] dimenzija u kojoj æemo stablo raspona napraviti
  */
StabloRaspona::StabloRaspona(QVector<KrajnjaTackaDuzi> tacke, int dimenzija)
{
	this->m_dimenzija = dimenzija;
	m_korijen = sortiranjeTacaka(tacke, 1);
}

StabloRaspona::StabloRaspona() : m_dimenzija(0) {}

/************************************************************************/
/* @brief Pomoæna funkcija pomoæu koje podijelimo vektor krajnjih taèaka
  u dva vektora krajnjih taèaka
 @param[in] tacke vektor svih krajnjih taèaka duži koje ispitujemo
 @param[out] lijeve_tacke vektor krajnjih taèaka u koje cemo ubaciti sve lijeve taèke od srednje taèke vektora krajnjih taèaka
 @param[out] desne_tacke vektor krajnjih taèaka u koje cemo ubaciti sve desne taèke od srednje taèke vektora krajnjih taèaka */
/************************************************************************/
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

/************************************************************************/
/* @brief pomocna funkcija pomocu koje pravimo stablo raspona, ovo je ustvari algoritam
	napraviStabloRaspona
	@param [in] tacke vektor kranjih taèaka duži koje æemo pretraziti
	@param [in] dimenzija u kojoj æemo stablo raspona napraviti
	@return pointer na cvor stabla, korijen */
/************************************************************************/
StabloRaspona::pCvor StabloRaspona::napraviStablo(QVector<KrajnjaTackaDuzi> tacke, int dimenzija)
{
	pCvor cvor(new Cvor);
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
		cvor->vrijednost = tacke.at(0);
	}

	if(dimenzija + 1 <= this->m_dimenzija)
	{
		cvor->p_korijen_sljedece_dim = sortiranjeTacaka(tacke, dimenzija + 1);
	}
	return cvor;
}


/************************************************************************/
/* @brief pomocna funkcija koja se bavi preprocesiranjem, sortiranjem krajnjih taèaka
	prije nego sto pozovemo algoritam pravljenja stabla raspona
	@param [in] tacke vektor kranjih taèaka duži koje æemo pretraziti
	@param [in] dimenzija u kojoj æemo stablo raspona napraviti 
	@return pointer na cvor stabla, korijen */
/************************************************************************/
StabloRaspona::pCvor StabloRaspona::sortiranjeTacaka(QVector<KrajnjaTackaDuzi> tacke, int dimenzija)
{
	if (dimenzija == 2)
	{
		sort(tacke.begin(), tacke.end(), PoredjenjePoY());
	} 
	else
	{
		sort(tacke.begin(), tacke.end(), PoredjenjePoX());
	}
	return napraviStablo(tacke, dimenzija);
}

/************************************************************************/
/* @brief pomocna funkcija za algoritam pretrage stabla raspona, trazi cvor grananja
	gdje se put do x i x' razgranjava
	@param[in] korijen pokazivac na korijen stabla raspona
	@param[in] pocetak pocetak raspona pretrage
	@param[in] kraj kraj raspona pretrage
	@param[in] pointer na cvor grananja, razdvajanja*/
/************************************************************************/
StabloRaspona::pCvor StabloRaspona::razdvajajuciCvor(pCvor korijen, int pocetak, int kraj, int koord)
{
	pCvor v(korijen);
	while(!jelList(v) && (v->vrijednost.vratiKoordinatu(koord) >= kraj  || v->vrijednost.vratiKoordinatu(koord) <= pocetak))
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

/************************************************************************/
/* @brief public funkcija za pretragu raspona 
   @param[in] x-koordinata prve tacke
   @param[in] x-koordinata druge tacke
   @param[in] y-koordinata prve tacke
   @param[in] y-koordinata druge tacke*/
/************************************************************************/
void StabloRaspona::pretragaRaspona(int x1, int x2, int y1, int y2)
{
	rezultat.clear();
	dvodimenzionalnaPretragaRaspona(m_korijen, x1, x2, y1, y2);
}

/************************************************************************/
/* @brief pomocna funckija za pretragu jednodimenzionalnog stabla raspona
   @param[in] korijen pokazivac na korijen jednodimenzionalnog stabla raspona
   @param[in] pocetak pocetak raspona pretrage
   @param[in] kraj kraj raspona pretrage*/
/************************************************************************/
void StabloRaspona::jednoDimenzionalnaPretragaRaspona(pCvor korijen, int pocetak, int kraj)
{
	pCvor cvor_grananja = razdvajajuciCvor(korijen, pocetak, kraj, 2);
	pCvor v;

	if (jelList(cvor_grananja))
	{
		if(cvor_grananja->vrijednost.vratiKoordinatu(2) >= pocetak && cvor_grananja->vrijednost.vratiKoordinatu(2) <= kraj)
		{
			rezultat.push_back(cvor_grananja->vrijednost);
		}
	} 
	else
	{
		v = cvor_grananja->lijevo_dijete;
		while(!jelList(v))
		{
			if(pocetak <= v->vrijednost.vratiKoordinatu(2))
			{
				prijaviPodstablo(v->desno_dijete);
				v = v->lijevo_dijete;
			}
			else
			{
				v = v->desno_dijete;
			}
		}
		
		if(v->vrijednost.vratiKoordinatu(2) >= pocetak && v->vrijednost.vratiKoordinatu(2) <= kraj)
		{
			rezultat.push_back(v->vrijednost);
		}
		


		v = cvor_grananja->desno_dijete;
		while(!jelList(v))
		{
			if(v->vrijednost.vratiKoordinatu(2) <= kraj)
			{
				prijaviPodstablo(v->lijevo_dijete);
				v = v->desno_dijete;
			}
			else
			{
				v = v->lijevo_dijete;
			}
		}
		if(v->vrijednost.vratiKoordinatu(2) >= pocetak && v->vrijednost.vratiKoordinatu(2) <= kraj)
		{
			rezultat.push_back(v->vrijednost);
		}
		

	}
}

/************************************************************************/
/* @brief Pomocna funkcija koja prolazi kroz stablo raspona ukorijenjeno u datom cvoru
    i prijavljuje tacke pohranjene u njegovim listovima
	@param[in] cvor cvor podstabla stabla raspona odakle cemo krenuti pretragu da prijavimo sve listove*/
/************************************************************************/
void StabloRaspona::prijaviPodstablo(pCvor cvor)
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

/************************************************************************/
/* @brief Pomocna funckija koja ispituje da li je dati cvor list
   @param[in] v cvor koji ispitujemo da li je list
   @return bool koji kaze jel list ili nije*/
/************************************************************************/
bool StabloRaspona::jelList(pCvor v)
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

/************************************************************************/
/*@brief Pomocna funckija koja obavlja pretragu dvodimenzionalnog stabla raspona
  @param[in] korijen pokazivac na korijen 2D stabla raspona
  @param[in] x-koordinata prve tacke
  @param[in] x-koordinata druge tacke
  @param[in] y-koordinata prve tacke
  @param[in] y-koordinata druge tacke */
/************************************************************************/
void StabloRaspona::dvodimenzionalnaPretragaRaspona(pCvor korijen, int x1, int x2, int y1, int y2)
{
	pCvor cvor_grananja = razdvajajuciCvor(korijen, x1, x2, 1);
	pCvor v;

	if (jelList(cvor_grananja))
	{
		if(cvor_grananja->vrijednost.vratiKoordinatu(1) >= x1 && cvor_grananja->vrijednost.vratiKoordinatu(1) <= x2
			&& cvor_grananja->vrijednost.vratiKoordinatu(2) >= y1 && cvor_grananja->vrijednost.vratiKoordinatu(2) <= y2)
		{
			rezultat.push_back(cvor_grananja->vrijednost);
		}
	} 
	else
	{
		v = cvor_grananja->lijevo_dijete;
		while(!jelList(v))
		{
			if(x1 <= v->vrijednost.vratiKoordinatu(1))
			{
				jednoDimenzionalnaPretragaRaspona(v->desno_dijete->p_korijen_sljedece_dim, y1, y2);
				v = v->lijevo_dijete;
			}
			else
			{
				v = v->desno_dijete;
			}
		}
		
		if(v->vrijednost.vratiKoordinatu(1) >= x1 && v->vrijednost.vratiKoordinatu(1) <= x2
			&& v->vrijednost.vratiKoordinatu(2) >= y1 && v->vrijednost.vratiKoordinatu(2) <= y2)
		{
			rezultat.push_back(v->vrijednost);
		}

		

		v = cvor_grananja->desno_dijete;
		while(!jelList(v))
		{
			if(v->vrijednost.vratiKoordinatu(1) <= x2)
			{
				jednoDimenzionalnaPretragaRaspona(v->lijevo_dijete->p_korijen_sljedece_dim, y1, y2);
				v = v->desno_dijete;
			}
			else
			{
				v = v->lijevo_dijete;
			}
		}
		
		
		if(v->vrijednost.vratiKoordinatu(1) >= x1 && v->vrijednost.vratiKoordinatu(1) <= x2
			&& v->vrijednost.vratiKoordinatu(2) >= y1 && v->vrijednost.vratiKoordinatu(2) <= y2)
		{
			rezultat.push_back(v->vrijednost);
		}	
	}	
}