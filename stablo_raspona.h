#ifndef STABLORASPONA_H
#define STABLORASPONA_H

#include <QVector>
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

	/************************************************************************/
	/* @brief Privatna struktura koja predstavlja cvor Stabla Raspona       */
	/************************************************************************/
	struct Cvor
	{
	public:
		typedef boost::shared_ptr<Cvor> Cvor::pCvor;

		Cvor::pCvor lijevo_dijete;			/// Pokazatelj na lijevo dijete cvora
		Cvor::pCvor desno_dijete;			/// Pokazatelj na desno dijete cvora
		Cvor::pCvor p_korijen_sljedece_dim;	/// Pokazatelj na cvor koji je korijen sljedece dimenzije 

		KrajnjaTackaDuzi vrijednost;		/// Koordinata tacke
	};

private:
	QVector<KrajnjaTackaDuzi> rezultat;  /// Vektor krajnjih tacaka u koji pohranjujemo rezultat pretrage stabla raspona
	int m_dimenzija;					 /// Dimenzija stabla raspona koje pravimo tj. koje cemo pretraziti
	Cvor::pCvor m_korijen;				 /// Pokazivac na korijen stabla raspona

	/************************************************************************/
	/* @brief pomocna funkcija koja se bavi preprocesiranjem, sortiranjem 
		      krajnjih taèaka prije nego sto pozovemo algoritam pravljenja 
			  stabla raspona
	   @param [in] tacke vektor kranjih taèaka duži koje æemo pretraziti
	   @param [in] dimenzija u kojoj æemo stablo raspona napraviti 
	   @return pointer na cvor stabla, korijen								*/
	/************************************************************************/
	Cvor::pCvor sortiranjeTacaka(QVector<KrajnjaTackaDuzi> tacke, int dimenzija);

	/************************************************************************/
	/* @brief pomocna funkcija pomocu koje pravimo stablo raspona, ovo je 
			  ustvari algoritam napraviStabloRaspona
		@param [in] tacke vektor kranjih taèaka duži koje æemo pretraziti
		@param [in] dimenzija u kojoj æemo stablo raspona napraviti
		@return pointer na cvor stabla, korijen								*/
	/************************************************************************/
	Cvor::pCvor napraviStablo(QVector<KrajnjaTackaDuzi> tacke, int dimenzija);

	/************************************************************************/
	/* @brief Pomoæna funkcija pomoæu koje podijelimo vektor krajnjih taèaka
		u dva vektora krajnjih taèaka
		@param[in] tacke vektor svih krajnjih taèaka duži koje ispitujemo
		@param[in, out] lijeve_tacke vektor krajnjih taèaka u koje cemo 
						ubaciti sve lijeve taèke od srednje taèke vektora 
						krajnjih taèaka
		@param[in, out] desne_tacke vektor krajnjih taèaka u koje cemo 
						ubaciti sve desne taèke od srednje taèke vektora 
						krajnjih taèaka										*/
	/************************************************************************/
	void podijeliNiz(QVector<KrajnjaTackaDuzi> tacke, QVector<KrajnjaTackaDuzi>&
		lijeve_tacke, QVector<KrajnjaTackaDuzi>& desne_tacke);

	/************************************************************************/
	/* @brief pomocna funkcija za algoritam pretrage stabla raspona, trazi 
		      cvor grananja gdje se put do x i x' razgranjava
	   @param[in] korijen pokazivac na korijen stabla raspona
	   @param[in] pocetak pocetak raspona pretrage
	   @param[in] kraj kraj raspona pretrage
	   @param[in] pointer na cvor grananja, razdvajanja						*/
	/************************************************************************/
	Cvor::pCvor razdvajajuciCvor(Cvor::pCvor korijen, int pocetak, int kraj, int dimenzija);
	
	/************************************************************************/
	/* @brief Pomocna funkcija koja prolazi kroz stablo raspona ukorijenjeno
	          u datom cvoru i prijavljuje tacke pohranjene u njegovim 
			  listovima
	   @param[in] cvor cvor podstabla stabla raspona odakle cemo krenuti 
			      pretragu da prijavimo sve listove						    */
	/************************************************************************/
	void prijaviPodstablo(Cvor::pCvor cvor);

	/************************************************************************/
	/* @brief Pomocna funckija koja ispituje da li je dati cvor list
	   @param[in] v cvor koji ispitujemo da li je list
	   @return bool koji kaze jel list ili nije							    */
	/************************************************************************/
	bool jelList(Cvor::pCvor cvor);
	
	/************************************************************************/
	/* @brief Pomocna funckija koja obavlja pretragu stabla raspona
	   @param[in] korijen pokazivac na korijen 2D stabla raspona
	   @param[in] pocetak, tacka od koje pocinje pretraga
	   @param[in] kraj, tacka do koje pretraga ide
	   @param[in] dim, dimenzija u kojoj se vrsi pretraga, da li je pretraga 
				  po y, x koordinati ili nekoj drugoj koordinati			*/
	/************************************************************************/
	void pretragaRaspona(Cvor::pCvor korjen, KrajnjaTackaDuzi pocetak, 
		KrajnjaTackaDuzi kraj, int dim);

public:

	/************************************************************************/
	/* @ brief Konstruktor bez parametara */
	/************************************************************************/
	StabloRaspona();

	/************************************************************************/
	/*  @ brief Konstruktor Stabla Raspona
		@param [in] tacke vektor kranjih taèaka duži koje æemo pretraziti
		@param [in] dimenzija u kojoj æemo stablo raspona napraviti         */
	/************************************************************************/
	StabloRaspona(QVector<KrajnjaTackaDuzi> tacke, int dimenzija);

	/************************************************************************/
	/* @brief public funkcija za pretragu raspona 
	   @param[in] x-koordinata prve tacke
       @param[in] x-koordinata druge tacke
       @param[in] y-koordinata prve tacke
       @param[in] y-koordinata druge tacke									*/
    /************************************************************************/
	void pretragaRaspona(KrajnjaTackaDuzi pocetak, KrajnjaTackaDuzi kraj, int dim);

	/************************************************************************/
	/* @brief Funkcija koja vraca rezultat pretrage stabla raspona u 
		zadatom rasponu                                                     */
	/************************************************************************/
	QVector<KrajnjaTackaDuzi> vratiRezultat(){ return rezultat; }

};

#endif //STABLORASPONA_H
