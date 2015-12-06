#ifndef STABLODUZI_H
#define STABLODUZI_H

#include "strukture.h"
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <QLineF>
using namespace std;

#define NEGBESKONACNO -5000   // -beskonacno
#define POZBESKONACNO +5000	  // +beskonacno	

/************************************************************************/
/* Klasa stablo raspona, napravljena prema algoritmu opisanom u 
   magistarskom radu, pomaže u pretrazi prozora pretrage, ispituje da li 
   krajnje taèke duži leže u prozoru pretrage						    */
/************************************************************************/
class StabloDuzi
{

private:
	/************************************************************************/
	/* @brief Asocijativna struktura, strukture stablo duži, potrebna da 
			  izracunamo kanonske podskupove cvorova, to jeste u nju cemo
			  da pohranimo kanonske podskupove da bi omogucili laksu kasniju
			  pretragu*/
	/************************************************************************/
	struct AsocijativniCvor
	{
	public:
		ElementarniInterval m_elementarni_interval;		/// Elementarni intervali duzi
		int idDuzi;										/// Identifikacija duzi
		int vrijednostPresjeka;							/// Vrijednost presjeka granicne prave sa duzi koju ispitujemo

		/************************************************************************/
		/* @brief Pomocna funkcija koja postavlja vrijednost presjeka na zadanu
				  vrijednost
		   @param[in] v, int, vrijednost na koju cemo postaviti vrijednsot 
					  presjeka													*/
		/************************************************************************/
		void postaviVrijednost(int v){ this->vrijednostPresjeka = v; }

	};

	/************************************************************************/
	/* @brief Privatna struktura cvor, predstavlja cvor stabla duzi, po 
		      uzoru na binarno stablo pretrage								*/
	/************************************************************************/
	struct Cvor
	{
	public:
		typedef boost::shared_ptr<Cvor> Cvor::pCvor;

		Cvor::pCvor lijevo_dijete;								///	Pokazivac na lijevo dijete cvora u stablu duzi
		Cvor::pCvor desno_dijete;								/// Pokazivac na desno dijete cvora u stablu duzi
		QVector<AsocijativniCvor> m_AsocijativnaStruktura;		/// Vektor asocijativnih cvorova
		ElementarniInterval m_elementarni_interval;				/// Varijabla za cuvanje elementarnih intervala
		Cvor::pCvor p_korijen_sljedece_dim;						/// Pokazivac na korijen stabla sljedece dimenzije, koje je sortirano prema koordinati te dimenzije
	};

	/************************************************************************/
	/* @brief Struktura koju koristi qStableSort da se definise 
              rastuci poredak po vrijednosti presjeka asocijativnog cvora   */
	/************************************************************************/
	struct PoredjenjePoVrijednostiPresjeka
	{
		bool operator()(const StabloDuzi::AsocijativniCvor &a_cvor1, 
			const StabloDuzi::AsocijativniCvor &a_cvor2)
		{
			StabloDuzi::AsocijativniCvor c1, c2;
			c1 = a_cvor1;
			c2 = a_cvor2;
			return c1.vrijednostPresjeka < c2.vrijednostPresjeka;
		}
	};

private:
	int m_dimenzija;		/// Dimenzija stabla duzi, tj, dimenzija prostora od kojeg pravimo stablo duzi
	Cvor::pCvor m_korijen;	/// Pokazivac na korijen stabla duzi

	/************************************************************************/
	/*  @brief Pomocna funkcija koja se bavi preprocesiranjem, sortiranjem 
		       krajnjih taèaka prije nego sto pozovemo algoritam konstrukcije
			   stabla duzi, ovisno o dimenziji sortiramo tacke
		@param [in, out] tacke vektor kranjih taèaka duži koje æemo pretraziti
		@param [in] dim, dimenzija u kojoj æemo stablo duzi napraviti	*/
	/************************************************************************/
	void sortiranjeTacaka(QVector<KrajnjaTackaDuzi>& tacke, int dim);

	/************************************************************************/
	/* @brief Funkcija koja pravi binarno stablo od vektora elementarnih 
			  intervala algoritam slican pravljenju stabla raspona
	   @param[in] vekt_elem_intervala, QVector, vektor elementarnih intervala 
				  od kojih pravimo binarno stablo pretrage
	   @return pokazivac na korijen stabla duzi								*/
	/************************************************************************/
	Cvor::pCvor napraviStabloElementarnihIntervala(QVector<ElementarniInterval> 
		vekt_elem_intervala);

	/************************************************************************/
	/* @brief Funkcija koja kreira elementarne intervale od vektora krajnjih
		      tacaka duzi
	   @param[in] tacke, QVector, vektor krajnjih tacaka duzi
	   @param[in] dim, dimenzija u kojoj pravimo stablo duzi
	   @return Funckija vraca vektor elementarnih intervala					*/
	/************************************************************************/
	QVector<ElementarniInterval> kreirajElementarneIntervale(QVector<KrajnjaTackaDuzi> 
		tacke, int dim);

	/************************************************************************/
	/* @brief Funkcija koja je zaduzena da napravi stablo duzi prema 
		      algoritmu opisanom u djelu
	   @param[in] tacke, QVector, vektor krajnjih tacaka duzi od kojih pravimo
				  stablo duzi
	   @param[in] vekDuzi, QVector, vektor duzi koji ispitujemo
	   @param[in] dim, dimenzija u kojoj pravimo stablo duzi, dimenzija 
				  prostora koje cemo pretrazivati
	   @return pokazivac na korijen stabla duzi								*/
	/************************************************************************/
	Cvor::pCvor napraviStablo(QVector<KrajnjaTackaDuzi> tacke, QVector<Duz> vekDuzi,
		int dim);

	/************************************************************************/
	/* @brief Funkcija koja umece interval na najvisu moguc poziciju unutar 
		      stabla duzi
	   @param[in] cvor, pokazivac na korijen stabla duzi
	   @param[in] d, Duz, duz koju umecemo u stablo duzi
	   @param[in] dim, dimenzija u kojoj pravimo stablo duzi
	   @return pokazivac na korijen stabla duzi								*/
	/************************************************************************/
	Cvor::pCvor umetniInterval(Cvor::pCvor cvor, Duz d, int dim);

	/************************************************************************/
	/* @ brief Funkcija za kreiranje asocijativnog stabla za cvor stabla duzi
	   @param[in] cvor, pokazivac na korijen stabla duzi
	   @param[in] vekDuzi, QVector, vektor duzi koje ispitujemo
	   @param[in] dim, dimenzija u kojoj pravimo asocijativno stablo
	   @return pokazivac na korijen stabla duzi								*/
	/************************************************************************/
	Cvor::pCvor napraviAsocijativnoStablo(Cvor::pCvor cvor, QVector<Duz> vekDuzi, int dim);

	/************************************************************************/
	/* @brief Funkcija koja pretrazuje asocijativnu strukturu koja je 
	          povezana sa cvorom xStabloDuzi, i prijavljuje sve duzi koje 
			  sijeku pretragu
	   @param[in] korijen, pCvor, pokazivac na korijen stabla duzi
	   @param[in] start, int, startna vrijednost od koje pocinjemo pretragu 
				  asocijativnog stabla
	   @param[in] ispitna_vrijednost, int, vrijednost pretrage, to jeste ako 
				  ispitujemo raspon pretrage, granicna vrijednost prozora 
				  pretrage
	   @param[in] pocetak, int, vrijednost koordiante tacke pocetka prozora 
				  pretrage
	   @param[in] kraj, int, vrijednost koordiante tacke kraja prozora 
				  pretrage
	   @param[in, out] vekDuzi, QVector, vektor duzi koje ispitujemo
	   @param[in] dim, dimenzija u kojoj pravimo asocijativno stablo	    */
	/************************************************************************/
	void prijaviAsocStabloDuzi(Cvor::pCvor korijen, int start, int ispitna_vrijednost,
		int pocetak, int kraj, QVector<Duz>& vekDuzi, int dimenzija);

	/************************************************************************/
	/* @brief Funkcija koja pretrazuje stablo duzi i trazi sve duzi koje 
		      sijeku prozor pretrage   
	   @param[in] korijen, pCvor, pokazivac na korijen stabla duzi
	   @param[in] ispitna_vrijednost, int, vrijednost pretrage, to jeste ako 
			      ispitujemo raspon pretrage, granicna vrijednost prozora 
			      pretrage
	   @param[in] pocetak, int, vrijednost koordiante tacke pocetka prozora 
			      pretrage
	   @param[in] kraj, int, vrijednost koordiante tacke kraja prozora 
			      pretrage
	   @param[in, out] vekDuzi, QVector, vektor duzi koje ispitujemo
	   @param[in] dim, dimenzija u kojoj pravimo asocijativno stablo		*/
	/************************************************************************/
	void pretraziStabloDuzi(Cvor::pCvor korijen, int ispitna_vrijednost, 
		int pocetak, int kraj, QVector<Duz>& vekDuzi, int dimenzija);

	/************************************************************************/
	/* @brief Pomoæna funkcija pomoæu koje podijelimo vektor tacaka
			  u dva vektora tacaka
		@param [in] tacke Qvector, vektor tacakakoji dijelimo
		@param [out] lijeve_tacke vektor taèaka u koji cemo ubaciti sve 
			         lijeve taèke od srednje taèke vektora taèaka
		@param [out] desne_tacke vektor taèaka u koji cemo ubaciti sve 
					 desne taèke od srednje taèke vektora  taèaka			*/
	template <typename NekiTip>
	void podijeliNiz(QVector<NekiTip> tacke, QVector<NekiTip>& lijeve_tacke,
		QVector<NekiTip>& desne_tacke);

	/************************************************************************/
	/* @brief Funkcija koja raèuna logaritam po bazi 2
	   @param[in] n, double, broj ciji cemo logaritam izracunati po bazi 2  */
	/************************************************************************/   
	double Log2(double n){  return log((double)n) / log(2.); }

	/************************************************************************/
	/* @brief Funkcija koja racuna koolinearnost tri tacke P, Q i R
	   @param [in] px, int, x-koordinata P tacke
	   @param [in] py, int, y-koordinata P tacke
	   @param [in] qx, int, x-koordinata Q tacke
	   @param [in] qy, int, y-koordinata Q tacke
	   @param [in] rx, int, x-koordinata R tacke
	   @param [in] ry, int, y-koordinata R tacke
	   @return vraca vrijednost determinante, ako je jednaka nuli, onda su 
			   tacke koolinearne, ako je > 0 onda je tacka R iznad duzi koji
			   kreiraju tacke P i Q u suportnom je tacka R ispod te duzi	*/
	/************************************************************************/
	int Det(int px, int py, int qx, int qy, int rx, int ry)
	{ return (px - rx) * (qy - ry) - (py - ry) * (qx - rx); }
	
	
	
public:
	/************************************************************************/
	/*  @ brief Konstruktor Stabla Duzi
		@param [in] tacke vektor kranjih taèaka duzi koje æemo pretraziti
		@param [in] dimenzija u kojoj æemo stablo duzi napraviti
		@param [in] vekDuzi, QVector, vektor duzi koje ispitujemo			*/
	/************************************************************************/
	StabloDuzi(QVector<KrajnjaTackaDuzi> tacke, int dimenzija, QVector<Duz> vekDuzi);

	/************************************************************************/
	/* @ brief Konstruktor bez parametara */
	/************************************************************************/
	StabloDuzi::StabloDuzi() : m_dimenzija(0) {}

	/************************************************************************/
	/* @brief Funkcija koja pretrazuje stablo duzi i trazi sve duzi koje 
		      sijeku prozor pretrage   
	   @param [in] ispitna_vrijednost, int, vrijednost pretrage, to jeste ako 
			       ispitujemo raspon pretrage, granicna vrijednost prozora 
				   pretrage
	   @param [in] pocetak, int, vrijednost koordiante tacke pocetka prozora 
				   pretrage
	   @param [in] kraj, int, vrijednost koordiante tacke kraja prozora 
				  pretrage
	   @param [in, out] vekDuzi, QVector, vektor duzi koje ispitujemo
	   @param [in] dim, dimenzija u kojoj pravimo asocijativno stablo		*/
	/************************************************************************/
	void pretraziStabloDuzi(int ispitna_vrijednost, int pocetak, int kraj, 
		QVector<Duz>& vekDuzi, int dimenzija);
	
};




#endif //STABLODUZI_H