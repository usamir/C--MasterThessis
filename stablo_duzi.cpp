#include "stablo_duzi.h"

StabloDuzi::StabloDuzi(QVector<KrajnjaTackaDuzi> tacke, int dimenzija, QVector<Duz> vekDuzi)
{
	m_dimenzija = dimenzija;
	m_korijen = napraviStablo(tacke, vekDuzi, 1);
}


 StabloDuzi::Cvor::pCvor StabloDuzi::napraviStablo(QVector<KrajnjaTackaDuzi> tacke, QVector<Duz> vekDuzi, int dim)
 {
 	sortiranjeTacaka(tacke, dim);
 	QVector<ElementarniInterval> vekt_elem_intervala = kreirajElementarneIntervale(tacke, dim);
 	Cvor::pCvor cvor = napraviStabloElementarnihIntervala(vekt_elem_intervala);
  	BOOST_FOREACH(Duz duz, vekDuzi)
  	{
  		cvor = umetniInterval(cvor, duz, dim);
  	}
	cvor = napraviAsocijativnoStablo(cvor, vekDuzi, dim);

	if(dim + 1 <= this->m_dimenzija)
	{
		cvor->p_korijen_sljedece_dim = napraviStablo(tacke, vekDuzi, dim + 1);
	}
	return cvor;
 }


QVector<ElementarniInterval> StabloDuzi::kreirajElementarneIntervale(QVector<KrajnjaTackaDuzi> tacke, int dim)
{
	int n = tacke.size();
	ElementarniInterval el_int;
	QVector<ElementarniInterval> vekt_elem_intervala;

	//kreiranje elemntarni intervala
	//prvi ekstrem
	el_int.pocetak = NEGBESKONACNO;
	el_int.kraj = tacke[0].vratiKoordinatu(dim);
	vekt_elem_intervala.push_back(el_int);

	for (int i = 0; i < n-1; i++)
	{
		//zatvoreni interval
		el_int.pocetak = tacke[i].vratiKoordinatu(dim);
		el_int.kraj = tacke[i].vratiKoordinatu(dim);
		vekt_elem_intervala.push_back(el_int);

		//otvoreni interval
		el_int.pocetak = tacke[i].vratiKoordinatu(dim);
		el_int.kraj = tacke[i + 1].vratiKoordinatu(dim);
		vekt_elem_intervala.push_back(el_int);
	}
	//zadnji zatvoreni interval
	el_int.pocetak = tacke[n - 1].vratiKoordinatu(dim);
	el_int.kraj = tacke[n - 1].vratiKoordinatu(dim);
	vekt_elem_intervala.push_back(el_int);

	//zadnji otvoreni interval
	el_int.kraj = POZBESKONACNO;
	vekt_elem_intervala.push_back(el_int);
	return vekt_elem_intervala;
}


void StabloDuzi::sortiranjeTacaka(QVector<KrajnjaTackaDuzi>& tacke, int dim)
{
	if (dim == 2)
	{
		sort(tacke.begin(), tacke.end(), PoredjenjePoY());
	} 
	else if (dim == 1)
	{
		sort(tacke.begin(), tacke.end(), PoredjenjePoX());
	}
}

template <typename NekiTip>
void StabloDuzi::podijeliNiz(QVector<NekiTip> tacke, QVector<NekiTip>& lijeve_tacke, QVector<NekiTip>& desne_tacke)
{
	int n = tacke.size();
	int n_lijevo = n - n / 2;

	int index(0);
	BOOST_FOREACH(NekiTip nt, tacke)
	{
		if(index < n_lijevo)
		{
			lijeve_tacke.push_back(nt);
		}
		else
		{
			desne_tacke.push_back(nt);
		}
		index++;
	}
}


StabloDuzi::Cvor::pCvor 
	StabloDuzi::napraviStabloElementarnihIntervala(QVector<ElementarniInterval> vekt_elem_intervala)
{
	int n = vekt_elem_intervala.size();
	Cvor::pCvor cvor(new Cvor);
	cvor->m_elementarni_interval.pocetak = vekt_elem_intervala.at(0).pocetak;
	cvor->m_elementarni_interval.kraj = vekt_elem_intervala.at(n-1).kraj;
	if(n > 1)
	{
		QVector<ElementarniInterval> lijeve_tacke;
		QVector<ElementarniInterval> desne_tacke;
		podijeliNiz(vekt_elem_intervala, lijeve_tacke, desne_tacke);
		cvor->lijevo_dijete = napraviStabloElementarnihIntervala(lijeve_tacke);
		cvor->desno_dijete = napraviStabloElementarnihIntervala(desne_tacke);
	}
	else
	{
		/// U slucaju lista da kreiramo list binarnog stabla
		cvor->m_elementarni_interval = vekt_elem_intervala.at(0);
	}
	return cvor;
}


StabloDuzi::Cvor::pCvor StabloDuzi::umetniInterval(Cvor::pCvor cvor, Duz d, int dim)
{
	int pocetakDuzi, krajDuzi;
	AsocijativniCvor assocStruktura;
	pocetakDuzi = d.vratiPocetnuKoordinatu(dim);
	krajDuzi = d.vratiKrajnjuKoordinatu(dim);
	
	// Mjenjam poziciju tacaka duzi da pocetna tacka duzi bude u odnosu na 
	// dimezniju koju pravim 'ispred' krajnje tacke duzi
	int pom(0);
	if(pocetakDuzi > krajDuzi)
	{
		pom = pocetakDuzi;
		pocetakDuzi = krajDuzi;
		krajDuzi = pom;
	}

	int pocetakIntervalaCvora, krajIntervalaCvora;
	pocetakIntervalaCvora = cvor->m_elementarni_interval.pocetak;
	krajIntervalaCvora = cvor->m_elementarni_interval.kraj;

	// Provjera da li se duz prostire kroz citavu 'plocu' cvora stabla duzi
	/* Duž je u kanononskom podskupu èvora v  ako u potpunosti prolazi preko 
	ploèe koja odgovara èvoru v - kažemo da se duž prostire ploèom – ali ne
	ploèom koja odgovara roditelju èvora v. 
	*/
	if((pocetakDuzi <= pocetakIntervalaCvora) && (krajIntervalaCvora <= krajDuzi))
	{
		int pocetakDuzipom, krajDuzipom;
		assocStruktura.idDuzi = d.vratiIdDuzi();
		if (dim == 1)
		{
			pocetakDuzipom = d.vratiPocetnuKoordinatu(dim + 1);
			krajDuzipom = d.vratiKrajnjuKoordinatu(dim + 1);	
		}
		else if (dim == 2)
		{
			pocetakDuzipom = d.vratiPocetnuKoordinatu(dim - 1);
			krajDuzipom = d.vratiKrajnjuKoordinatu(dim - 1);
		}
		// Pokusavamo da djelimicno zadrzimo vertikalni ili horizontalni poredak, ovisno o dimenziji
		if(pocetakDuzipom > krajDuzipom)
		{
			pom = pocetakDuzipom;
			pocetakDuzipom = krajDuzipom;
			krajDuzipom = pom;
		}

		assocStruktura.m_elementarni_interval.pocetak = pocetakDuzipom;
		assocStruktura.m_elementarni_interval.kraj = krajDuzipom;
		cvor->m_AsocijativnaStruktura.push_back(assocStruktura);

	}
	// Ili ako se duz ne prostire kroz citavu 'plocu' ovog cvora stabla duzi, 
	// da li se prostire kroz 'ploce' njegove djece
	else
	{
		if (cvor->lijevo_dijete != NULL)
		{
			int donjaGranicaLijevogDijeteta, donjaGranicaDesnogDijeteta, 
				gornjaGranicaLijevogDijeteta, gornjaGranicaDesnogDijeteta;
			donjaGranicaLijevogDijeteta = cvor->lijevo_dijete->m_elementarni_interval.pocetak;
			gornjaGranicaLijevogDijeteta = cvor->lijevo_dijete->m_elementarni_interval.kraj;
			donjaGranicaDesnogDijeteta = cvor->desno_dijete->m_elementarni_interval.pocetak;
			gornjaGranicaDesnogDijeteta = cvor->desno_dijete->m_elementarni_interval.kraj;

			if( donjaGranicaLijevogDijeteta<= krajDuzi && pocetakDuzi <= gornjaGranicaLijevogDijeteta)
			{
				umetniInterval(cvor->lijevo_dijete, d, dim);
			}

			if( donjaGranicaDesnogDijeteta <= krajDuzi && pocetakDuzi <= gornjaGranicaDesnogDijeteta)
			{
				umetniInterval(cvor->desno_dijete, d, dim);
			}
		}
		
	}
	return cvor;
}

StabloDuzi::Cvor::pCvor StabloDuzi::napraviAsocijativnoStablo(Cvor::pCvor cvor,
	QVector<Duz> vekDuzi, int dim)
{

	int granica = cvor->m_elementarni_interval.kraj;
	Duz d;
	QPointF start, end, presjek, pocetakDuzi, krajDuzi;
	QLineF prava, duz;
	int n = cvor->m_AsocijativnaStruktura.size();
	if (n == 0)
	{
		if (cvor->lijevo_dijete != NULL && cvor->desno_dijete != NULL)
		{
			napraviAsocijativnoStablo(cvor->lijevo_dijete, vekDuzi, dim);
			napraviAsocijativnoStablo(cvor->desno_dijete, vekDuzi, dim);
		}
	}
	if (dim == 1)
	{
		start.setX(granica);
		start.setY(NEGBESKONACNO);
		end.setX(granica);
		end.setY(POZBESKONACNO);	
	} 
	else if (dim == 2)
	{
		start.setY(granica);
		start.setX(NEGBESKONACNO);
		end.setY(granica);
		end.setX(POZBESKONACNO);
	}
	// Konstrukcija granicne prave
	prava.setP1(start);
	prava.setP2(end);
	
	for (int i = 0; i < n; i++)
	{
		d = vekDuzi[cvor->m_AsocijativnaStruktura.at(i).idDuzi];

		pocetakDuzi.setX(d.vratiPocetnuKoordinatu(1));
		pocetakDuzi.setY(d.vratiPocetnuKoordinatu(2));
		krajDuzi.setX(d.vratiKrajnjuKoordinatu(1));
		krajDuzi.setY(d.vratiKrajnjuKoordinatu(2));
		duz.setP1(pocetakDuzi);
		duz.setP2(krajDuzi);

		// Racunamo presjek prave sa duzi koju ispitujemo
		duz.intersect(prava, &presjek);
		if (dim == 1)
		{
			cvor->m_AsocijativnaStruktura[i].postaviVrijednost((int)presjek.y());
		}
		else if (dim == 2)
		{
			cvor->m_AsocijativnaStruktura[i].postaviVrijednost((int)presjek.x());
		}
		
	}

	qStableSort(cvor->m_AsocijativnaStruktura.begin(), cvor->m_AsocijativnaStruktura.end(), PoredjenjePoVrijednostiPresjeka());

	
	int razinaListovi = (int)floor(Log2((double)n)); /// na kojoj razini su listovi
	int brCvNaRazini = (int)pow(2.0, razinaListovi); /// broj listova na toj razini
	int razinaCvorova= n - brCvNaRazini;
	int brojListova = razinaCvorova * 2;

	// Kreiranje binarnog stabla 'top-bottom' tehnikom, asociajtivna struktura ce biti kreirana kao binarno stablo
	QVector<AsocijativniCvor> pomStablo;
	AsocijativniCvor asocCvor;

	if(brojListova > 0)
	{
		for(int i = brojListova; i <= n-1; i++)
		{
			pomStablo.push_back(cvor->m_AsocijativnaStruktura.at(i));
		}
		for(int i = 0; i <= brojListova-1; i++)
		{
			pomStablo.push_back(cvor->m_AsocijativnaStruktura.at(i));
		}
	}else
	{
		pomStablo = cvor->m_AsocijativnaStruktura;
	}


	int lijevoDijete = n - 2;
	int desnoDijete = n - 1;
	for(int i = n - 1; i > 0; i--)
	{
		asocCvor.m_elementarni_interval.pocetak = pomStablo[lijevoDijete].m_elementarni_interval.pocetak;
		asocCvor.m_elementarni_interval.kraj = pomStablo[desnoDijete].m_elementarni_interval.kraj;
		asocCvor.idDuzi = -1;

		pomStablo.push_front(asocCvor);
		lijevoDijete--;
		desnoDijete--;
	}

	for(int i=0;i <= n-2; i++)
	{
		int index = i;
		index = index * 2 + 1;
		while(pomStablo[index].idDuzi == -1)
		{
			index = index * 2 + 2;
		}


		pomStablo[i].vrijednostPresjeka = pomStablo[index].vrijednostPresjeka;
	}

	cvor->m_AsocijativnaStruktura.clear();
	cvor->m_AsocijativnaStruktura = pomStablo;

	return cvor;
}


void StabloDuzi::prijaviAsocStabloDuzi(Cvor::pCvor korijen, int start, int ispitna_vrijednost,
	int pocetak, int kraj, QVector<Duz>& vekDuzi, int dimenzija)
{
	Duz d;
	int id_duzi = korijen->m_AsocijativnaStruktura[start].idDuzi;
	
	if(id_duzi != -1) //ako je list
	{
		int startx, starty, endx, endy, pom;
		d = vekDuzi[id_duzi];
		startx = d.vratiPocetnuKoordinatu(1);
		starty = d.vratiPocetnuKoordinatu(2);
		endx = d.vratiKrajnjuKoordinatu(1);
		endy = d.vratiKrajnjuKoordinatu(2);

		if (dimenzija == 1)
		{
			// Mjenja poziciji da bude u rastucem poretku
			if(startx > endx)
			{
				pom = startx;
				startx = endx;
				endx = pom;
				pom = starty;
				starty = endy;
				endy = pom;
			}
 
			// Ako je tacka sa koordinatama ispitna_vrijednost i pocetak ispod linije koju kreiraju prve dvije tacke 
			// i ako je tacka sa koordinatama ispitna_vrijednost i kraj iznad linije koju kreiraju prve dvije tacke
			if((Det(startx, starty, endx, endy, ispitna_vrijednost, pocetak) <=0 ) && 
				(Det(startx, starty, endx, endy, ispitna_vrijednost, kraj) >=0 ))
			{
				vekDuzi[id_duzi].oznaciDuz();
			}
		}
		else if(dimenzija == 2)
		{
			if(starty > endy)
			{
				pom = starty;
				starty = endy;
				endy = pom;
				pom = startx;
				startx = endx;
				endx = pom;
			}

			// Ako je tacka sa koordinatama pocetak i ispitna_vrijednost iznad linije koju kreiraju prve dvije tacke 
			// i ako je tacka sa koordinatama kraj i ispitna_vrijednost ispod linije koju kreiraju prve dvije tacke 
			if((Det(startx, starty, endx, endy, pocetak, ispitna_vrijednost) >=0 ) && 
				(Det(startx, starty, endx, endy, kraj, ispitna_vrijednost) <=0))
			{
				vekDuzi[id_duzi].oznaciDuz();
			}

		}
	}
	else
	{
		int donja_granica_lijevog_dijeteta, donja_granica_desnog_dijeteta, 
			gornja_granica_lijevog_dijeteta, gornja_granica_desnog_dijeteta;
		donja_granica_lijevog_dijeteta = korijen->m_AsocijativnaStruktura[start*2+1].m_elementarni_interval.pocetak;
		gornja_granica_lijevog_dijeteta = korijen->m_AsocijativnaStruktura[start*2+1].m_elementarni_interval.kraj;
		donja_granica_desnog_dijeteta = korijen->m_AsocijativnaStruktura[start*2+2].m_elementarni_interval.pocetak;
		gornja_granica_desnog_dijeteta = korijen->m_AsocijativnaStruktura[start*2+2].m_elementarni_interval.kraj;

		if(donja_granica_lijevog_dijeteta <= kraj && pocetak <= gornja_granica_lijevog_dijeteta)
		{
			prijaviAsocStabloDuzi(korijen, start * 2 + 1, ispitna_vrijednost,  
				pocetak,  kraj, vekDuzi, dimenzija);
		}
		if(donja_granica_desnog_dijeteta <= kraj && pocetak <= gornja_granica_desnog_dijeteta)
		{
			prijaviAsocStabloDuzi(korijen, start * 2 + 2, ispitna_vrijednost, 
				pocetak,  kraj, vekDuzi, dimenzija);
		}

	}


}

void StabloDuzi::pretraziStabloDuzi(Cvor::pCvor korijen, int ispitna_vrijednost,
	int pocetak, int kraj, QVector<Duz>& vekDuzi, int dimenzija)
{
	if( korijen->m_AsocijativnaStruktura.size() > 0)
	{
		prijaviAsocStabloDuzi(korijen, 0, ispitna_vrijednost, pocetak, kraj, vekDuzi, dimenzija);
	}

	if(korijen->lijevo_dijete != NULL && korijen->desno_dijete != NULL)
	{
		if(korijen->lijevo_dijete->m_elementarni_interval.pocetak <= ispitna_vrijednost && 
			ispitna_vrijednost <= korijen->lijevo_dijete->m_elementarni_interval.kraj )
		{
			pretraziStabloDuzi(korijen->lijevo_dijete, ispitna_vrijednost, pocetak, kraj, vekDuzi, dimenzija);
		}
		else
		{
			pretraziStabloDuzi(korijen->desno_dijete, ispitna_vrijednost, pocetak, kraj, vekDuzi, dimenzija);
		}
	}
}

void StabloDuzi::pretraziStabloDuzi(int ispitna_vrijednost, int pocetak, int kraj,
	QVector<Duz>& vekDuzi, int dimenzija)
{
	if (dimenzija == 1)
	{
		pretraziStabloDuzi(m_korijen, ispitna_vrijednost, pocetak, kraj, vekDuzi,
			dimenzija); 
	}
	else if (dimenzija == 2)
	{
		pretraziStabloDuzi(m_korijen->p_korijen_sljedece_dim, ispitna_vrijednost, 
			pocetak, kraj, vekDuzi, dimenzija); 
	}
}