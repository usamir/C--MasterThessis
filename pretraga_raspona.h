#ifndef PRETRAGARASPONA_H
#define PRETRAGARASPONA_H

#include <iostream>
#include <QTextStream>
#include <QVector>
#include <QVectorIterator>
#include "strukture.h"
#include <QFileDialog>
#include <QPainter>
#include <QPoint>
#include <QRubberBand>
#include <QMouseEvent>
#include <QMessageBox>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QtAlgorithms>
#include <math.h>
#include "stablo_raspona.h"
#include "stablo_duzi.h"

#define NEGBESKONACNO -5000   /// minus beskonacno
#define POZBESKONACNO +5000   /// plus beskonacno

/************************************************************************/
/* @brief Stanja u kojim aplikacija moze biti                           */
/************************************************************************/
enum state { CEKANJA, PRETRAGA, CRTANJA1, CRTANJA2 }; 

/************************************************************************/
/* @brief Klasa odgovorna za GUI aplikacije								*/
/************************************************************************/
class PretragaProstora : public QWidget 
{
	Q_OBJECT

public:
    state trenutnoStanje;										/// Trenutno stanje aplikacije
    QPoint pocetak_prozora;										/// Pocetna taèka prozora pretrage
    QPoint start;												/// Polazna taèka duži u fazi kada ih kreiramo
    QRubberBand *izbor;											/// Izabrani raspon
    QVector<Duz> vektorDuzi;									/// Vektor sa nacrtanim dužima
    QVector<KrajnjaTackaDuzi> vektorKrajnjihTacaka;			    /// Vektor krajnjih taèaka duži
    QPen crna;													/// Boja za oznacenu duzu
	QPen crvena;												/// Boja za neoznacenu duz
	StabloRaspona m_rtree;									    /// Instanca klase stablo raspona
	StabloDuzi m_stabloDuzi;									/// Instanca klase stablo duzi

public:

    /************************************************************************/
    /* @brief Konstruktor klase PretragaProstora, koji je ustvari extended 
	    konstruktor klase QWidget 
	   @param[in] parent, QWidget varijabla, nula je varijabla jer da je 
	    nenulta varijabla onda bi moj widget bio 'rendered' unutar 'parent'
		widgeta, te bi zajedno sa 'parent' widgetom bio unisten kada 'parent'
		je unisten															*/
    /************************************************************************/
    explicit PretragaProstora(QWidget *parent = 0);

    /************************************************************************/
    /* @brief Destruktor klase												*/
    /************************************************************************/
    ~PretragaProstora();

	/************************************************************************/
	/* @brief Overrided metoda koja definise akciju mousePressEvent-a       
	   @param[in] dogadjaj, QMouseEvent, parametar koji opisuju dogajdaj 
				  misa, u ovom slucaju da li je dugme misa pritisnuto		*/
	/************************************************************************/
    void mousePressEvent(QMouseEvent *dogadjaj);

	/************************************************************************/
	/* @brief overriding metod akcije pomjeranja misa
	   @param[in] dogadjaj, QMouseEvent, parametar koji opisuju dogajdaj 
				  misa, u ovom slucaju da li je mis pomjeren				*/
	/************************************************************************/
    void mouseMoveEvent(QMouseEvent *dogadjaj);

	/************************************************************************/
	/* @brief Overriding metod kada "otpustimo" mis, ovdje je implementirana 
			  logika sta se desava kada smo u stanju pretrazivanja, koje se 
			  duzi nalaze u rasponu	
	   @param[in] dogadjaj, QMouseEvent, parametar koji opisuju dogajdaj 
				  misa, u ovom slucaju da li je dugme misa 'otpusteno'		*/
	/************************************************************************/
    void mouseReleaseEvent(QMouseEvent *dogadjaj);

	/************************************************************************/
	/* @brief Overriding metoda koja rukovodi sa crtanjem GUI objekata 
	   @param[in] dogadjaj QPaintEvent, parametar koji opisuju dogadjanja sa 
			      crtanjem stavki u widgetu									*/
	/************************************************************************/ 
    void paintEvent(QPaintEvent *dogadjaj);

	/************************************************************************/
	/* @brief Funkcija koja se bavi crtanjem graficki objekata, 
			  uglavnom duzi
	   @param[in] painter, QPainter, klasa zadudzena za low-level crtanje u 
				  widgetu													*/
	/************************************************************************/
    void crtaj(QPainter *painter);

	/************************************************************************/
	/* @brief Funckija koja iz ranije sesije, kupi spasene duzi iz fajla, tj.
			  duzi koje su bile nacrtane cita iz fajla i upisuje u vektor_duzi
	   @param[in] ime_fajla, QString, ime fajla u koji cemo upisati, trenutan 
				  poredak i broj duzi										*/
	/************************************************************************/
    bool kreirajIzFajla(QString ime_fajla);

	/************************************************************************/
	/* @brief Funkcija koja cita iz vektora_duzi i upisuje u fajl duzi koje su
		      nacrtane, za buduca koristenja 
	   @param[in] ime_fajla, QString, ime fajla u koji cemo upisati, trenutan 
	              poredak i broj duzi										*/
	/************************************************************************/
    bool spasiUFajl(QString ime_fajla);

    /************************************************************************/
	/* @brief Funkcija koja prolazi kroz sve elemente vektora duži i
			  postavlja status duži na 'not selected', tj ponistava prethodni 
			  odabir duzi													*/
	/************************************************************************/
    void ponistiOdabirDuzi();


private slots:

	 /************************************************************************/
	 /* @brief Funkcija koja objasnjava akcije koje se desavaju kada je 
			   aplikacija u stanju crtanja duzi								 */
	 /************************************************************************/
     void idiUStanjeCrtanja();

	 /************************************************************************/
	 /* @brief Funkcija koja objasnjava akcije koje se desavaju kada je 
		       aplikacija u stanju pretrage, tj. kada trazimo koje duzi leze 
			   u rasponu													 */
	 /************************************************************************/
     void idiUStanjePretrage();

	 /************************************************************************/
     /* @brief Funkcija koja objasnjava akcije koje se desavaju kada resetujemo 
			   aplikaciju													 */
	 /************************************************************************/
     void reset();

	 /************************************************************************/
	 /* @brief Funkcija koja objasnjava akcije koje se desavaju kada otvaramo 
			   fajl															 */
	 /************************************************************************/
     void otvoriFajl();

	 /************************************************************************/
	 /* @brief Funkcija koja objasnjava akcije koje se desavaju kada spasavamo 
			   fajl															 */
	 /************************************************************************/
     void spasiFajl();

	 /************************************************************************/
	 /* @brief Funkcija koja ispisuje informacije o aplikaciji				 */
	 /************************************************************************/
	 void informacije();

	 /************************************************************************/
	 /* @brief Funkcija koja ispisuje kako se koristi aplikacija			 */
	 /************************************************************************/
	 void pomoc();

signals:

	 /************************************************************************/
	 /* @brief Signal da li je pretraga, signal je metoda koju æemo emitovati
			   a ne izvršiti 												 */
	 /************************************************************************/
     void jelPretraga(bool b);

	 /************************************************************************/
	 /* @brief Signal da li je crtanje										 */
	 /************************************************************************/
     void jelCrtanje(bool b);
};

#endif // PRETRAGARASPONA_H
