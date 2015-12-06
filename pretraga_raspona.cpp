#include "pretraga_raspona.h"


PretragaProstora::PretragaProstora(QWidget *parent) : QWidget(parent)
{
	crna = QPen(Qt::black,1);
	crvena = QPen(Qt::red,2);

	izbor = new QRubberBand(QRubberBand::Rectangle, this);

	trenutnoStanje = CEKANJA;
}

PretragaProstora::~PretragaProstora()
{
}

void PretragaProstora::paintEvent(QPaintEvent *dogadjaj)
{
    QPainter crtac(this);
    crtac.setRenderHint(QPainter::Antialiasing, true);
    crtaj(&crtac);
}


void PretragaProstora::mousePressEvent(QMouseEvent *dogadjaj)
{
	//Akcija mousePress zavisi u kojem stanju je aplikacija
    switch(trenutnoStanje)
	{
        case PRETRAGA:
        {
			pocetak_prozora = dogadjaj->pos(); 
			izbor->setGeometry(QRect(pocetak_prozora, QSize()));
			izbor->show();                
			ponistiOdabirDuzi();
			update();
			break;
        }
        
		/* Stanje kada aplikacija čeka koordiante početka duži koje crtamo, pohrani 
		 * u memoriju te koordinate
		 */
        case CRTANJA1:
		{	 
			start = dogadjaj->pos();
			trenutnoStanje = CRTANJA2;
			break;
        }

		/* Stanje kada aplikacija čeka koordiante druge krajnje tačke duži koje 
		 * crtamo, pohrani u memoriju te koordinate
		 */ 
        case CRTANJA2:
        {
			Duz s;
			s.postaviPocetnuKoordinatu(start);
			s.postaviKrajnjuKoordinatu(dogadjaj->pos());
			s.ponistiOznacenuDuz();
			vektorDuzi.push_back(s);
			trenutnoStanje = CRTANJA1;
			update();
			break;
        }
    
        default:
                break;
        }
}


void PretragaProstora::mouseMoveEvent(QMouseEvent *dogadjaj)
{
        if(trenutnoStanje == PRETRAGA)
		{
            izbor->setGeometry(QRect(pocetak_prozora, dogadjaj->pos()).normalized());
        }
}


void PretragaProstora::mouseReleaseEvent(QMouseEvent *dogadjaj)
    {
        if(trenutnoStanje == PRETRAGA)
		{
            int x1 = izbor->x();
            int y1 = izbor->y();
            int x2 = x1+izbor->width();
            int y2 = y1+izbor->height();

			KrajnjaTackaDuzi pocetak, kraj;
			QPoint prva_tacka, druga_tacka;
			static int dimenzija = 2;
			prva_tacka.setX(x1);
			prva_tacka.setY(y1);
			druga_tacka.setX(x2);
			druga_tacka.setY(y2);
			pocetak.postaviKoordinatu(prva_tacka);
			kraj.postaviKoordinatu(druga_tacka);

			m_rtree.pretragaRaspona(pocetak, kraj, dimenzija);
			QVector<KrajnjaTackaDuzi> rezultat = m_rtree.vratiRezultat();
			QVector<KrajnjaTackaDuzi>::iterator it = rezultat.begin();
			for (it; it != rezultat.end(); ++it)
			{
				int i = it->vratiIdDuzi();
				if(!(vektorDuzi[i].jelOznacena()))
				{
					vektorDuzi[i].oznaciDuz();	
				}
			}		

			m_stabloDuzi.pretraziStabloDuzi(x1, y1, y2, vektorDuzi, 1);
			m_stabloDuzi.pretraziStabloDuzi(x2, y1, y2, vektorDuzi, 1);

			m_stabloDuzi.pretraziStabloDuzi(y1, x1, x2, vektorDuzi, 2);
			m_stabloDuzi.pretraziStabloDuzi(y2, x1, x2, vektorDuzi, 2);
	
            update();
			
            
        }
    }

void PretragaProstora::crtaj(QPainter *painter)
{
    QVectorIterator<Duz> it(vektorDuzi);
    while (it.hasNext())
	{
        Duz duz = it.next();
        if(duz.jelOznacena())
		{
            painter->setPen(crvena);
		}
        else
		{
            painter->setPen(crna);
		}
        painter->drawLine(duz.vratiPocetnuKoordinatu(), duz.vratiKrajnjuKoordinatu());
    }
}


bool PretragaProstora::kreirajIzFajla(QString filename)
{
    QFile file(filename);
    int x1, y1, x2, y2;

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		return false;
	}
    QTextStream in(&file);
    vektorDuzi.clear();
    while (!in.atEnd()) 
	{
        QString line = in.readLine();
		// Podaci su razmaknuti 'spaceom'
        QStringList fields = line.split(' ');
		// Kupi podatke iz fajla i pohranjuje koordiante u 'int' varijable
        if (fields.size() == 4) 
		{
           
            x1 = fields.at(0).toInt();
            y1 = fields.at(1).toInt();

            x2 = fields.at(2).toInt();
            y2 = fields.at(3).toInt();

            Duz s;
            s.postaviPocetnuKoordinatu(QPoint(x1,y1));
            s.postaviKrajnjuKoordinatu(QPoint(x2,y2));
            s.ponistiOznacenuDuz();
            vektorDuzi.push_back(s);
        }
    }

        file.close();
        return true;
}

bool PretragaProstora::spasiUFajl(QString filename)
{
    
    QFile file(filename);
    int x1, y1, x2, y2, brojDuzi;

    if (!file.open(QIODevice::WriteOnly)) 
	{
        std::cerr << "Ne moze da otvori fajl za upisivanje: " << 
			qPrintable(file.errorString()) << std::endl;
        return false;
    }            
    QTextStream out(&file);
    brojDuzi = vektorDuzi.size();
    for(int i=0; i < brojDuzi; i++)
    {
        x1 = vektorDuzi[i].vratiPocetnuKoordinatu(1);
        y1 = vektorDuzi[i].vratiPocetnuKoordinatu(2);
        x2 = vektorDuzi[i].vratiKrajnjuKoordinatu(1);
        y2 = vektorDuzi[i].vratiKrajnjuKoordinatu(2);
        // upisuj u fajl koordinate i odvajamo ih sa 'spaceom' da ih moze lakse parsati kada ih citamo
        out << x1 << " "<< y1 << " "<< x2 << " "<< y2<<endl;
    }

    file.close();
    return true;
}

void PretragaProstora::ponistiOdabirDuzi()
{
	for (int i = 0; i < vektorDuzi.size(); i++)
	{
		vektorDuzi[i].ponistiOznacenuDuz();
	}
}


void PretragaProstora::idiUStanjeCrtanja()
{ 
    if(trenutnoStanje != CRTANJA1)
	{
        emit jelPretraga(false);
        trenutnoStanje = CRTANJA1;
        izbor->hide();
        ponistiOdabirDuzi();
        update();
    }
}


void PretragaProstora::idiUStanjePretrage()
{
    if(trenutnoStanje != PRETRAGA)
	{

			// Stavljanje u vektor krajnjih tacaka, krajnje tacke duzi koje cemo ispitivati da li leze u rasponu
            vektorKrajnjihTacaka.clear(); // Ispraznimo vektor krajnjih tačaka
            int i(0);
            while (i < vektorDuzi.size())
			{
               vektorDuzi[i].postaviIDDuzi(i);
                
               KrajnjaTackaDuzi pocetna_tacka;
               pocetna_tacka.postaviIdDuzi(i);
               pocetna_tacka.postaviKoordinatu(vektorDuzi[i].vratiPocetnuKoordinatu());
               vektorKrajnjihTacaka.push_back(pocetna_tacka);

               KrajnjaTackaDuzi krajnja_tacka;
               krajnja_tacka.postaviIdDuzi(i);
               krajnja_tacka.postaviKoordinatu(vektorDuzi[i].vratiKrajnjuKoordinatu());
               vektorKrajnjihTacaka.push_back(krajnja_tacka);
                i++;
            }

            // Ako ima podataka u vektoru, mozemo krenuti sa kreiranjem stabla raspona i stabla duzi
            if(vektorKrajnjihTacaka.size() > 0)
			{
               
			  StabloRaspona rtree(vektorKrajnjihTacaka, 2);
			  m_rtree = rtree;
			  
 			  StabloDuzi stablo_duzi(vektorKrajnjihTacaka, 2, vektorDuzi);
 			  m_stabloDuzi = stablo_duzi;
            }
            emit jelCrtanje(false);
    }

    trenutnoStanje = PRETRAGA;
    izbor->hide();
}


void PretragaProstora::reset()
{
    vektorDuzi.clear();
    trenutnoStanje = CEKANJA;
    izbor->hide();    
    
    emit jelPretraga(false);
    emit jelCrtanje(false);
    update();
 }


void PretragaProstora::otvoriFajl()
{
        QString filename = QFileDialog::getOpenFileName( this, "Otvori listu duzi", "lista.slst", "Lista duzi (*.slst)");
        if(!filename.isNull())
        {
                if(!kreirajIzFajla(filename))
                {
					return;
                }

                trenutnoStanje = CEKANJA;
                izbor->hide();

                emit jelPretraga(false);
                emit jelCrtanje(false);
                update();
        }
}


void PretragaProstora::spasiFajl()
{
    QString filename = QFileDialog::getSaveFileName(this, "Spasi listu duzi", "lista.slst", "Lista duzi (*.slst)");
    if(!filename.isNull())
	{
            if(!spasiUFajl(filename))
			{
				return;
			}
            trenutnoStanje = CEKANJA;
    }
}

void PretragaProstora::informacije()
{
	QMessageBox::about(this, "O Magistarskoj aplikaciji",
		"Primjena algoritama za pretragu visedimenzionalnog prostora\n\n"
		"Copyright 2013 Samir Uzunovic ");
}

void PretragaProstora::pomoc()
{
	QMessageBox::about(this, "Aplikacija\n",
		"Odlaskom na meni Edit->Crtaj ili pritiskom na ikonu olovke mozemo da\n"
		"nacrtamo duzi koje cemo da pretrazimo\n\n"
		"Nacrtane duzi mozemo spasiti tako sto cemo otici na meni File->Spasi\n"
		"ili pritiskom na disketu \n\n"
		"Vec spasene duzi mozemo ucitati tako sto cemo ici na File->Otvori ili\n"
		"tako sto cemo otici an ikonu folder\n\n"
		"Dobijamo cistu plohu za crtanje tako sto cemo otici na meni File->Reset\n"
		"ili na pritiskom na ikonu papir\n\n"
		"Nacrtane duzi ili duzi koje smo ucitali na plohu za crtanje, mozemo\n"
		"pretraziti tako sto cemo na Edit->Pretraga ili na ikonu mape\n\n"
		"Info aplikaciji mozemo vidjeti tako sto cemo otici na Info->Informacije\n"
		"ili pristikom na ikonu informacija\n\n"
		"Izlazimo iz aplikacije na File->Izlaz ili na odlaskom na ikonu X.");
}
