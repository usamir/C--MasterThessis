//#include <QtGui/QApplication>
//#include "pretraga_raspona.h"
//#include "boost/filesystem.hpp"
//
//int main(int argc, char *argv[])
//{
    #include <iostream>
#include "boost/filesystem.hpp"
#include <unordered_map>
#include <string>

using namespace std;
using namespace boost::filesystem;

struct filedescription {
string fullpath;
string MD5;
};
typedef std::unordered_map<string, vector<filedescription>> file_list_type; // string: name+”#”+size, filedescription

void list_dir(string const& path, file_list_type& file_list) 
{
    boost::filesystem::path p (path);
    vector<filedescription >* existing_vector

    directory_iterator end_itr;

    // cycle through the directory
    for (directory_iterator itr(p); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path()))
        { // not directory
             string ref = itr->path().string() + “#” + std::to_string(itr->path().size());
             file_description file = {.fullpath = itr->path, .MD5 = NULL};
            file_list_type::const_iterator finder =  file_list.find(ref); 
            if(finder == file_list.end())
                file_list.insert(ref, new vector<file>);
            else
                finder->second.push_back(file);
                // cout << current_file << endl;
        } 
        else if (is_directory(itr->path())) 
        {
	        list_dir(itr->path(), file_list);
        }
    }
}

    /*QApplication a(argc, argv);

    QWidget container;

    container.setWindowTitle("Primjena algoritama za pretragu visedimenzionalnog prostora u kompjuterskoj geometriji");

    QMenuBar menu_bar(&container);
    QMenu open_menu("&File", &container);

    QAction otvori("&Otvori", &container);
    otvori.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    otvori.setIcon(QIcon(":/slike/otvori"));
    otvori.setToolTip(QString("Otvori fajl u kojem su liste duzi"));
    open_menu.addAction(&otvori);

    QAction spasi("&Spasi", &container);
    spasi.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    spasi.setIcon(QIcon(":/slike/spasi"));
    spasi.setToolTip(QString("Spasi u fajl u kojem ce biti lista duzi prezentovanih aplikaciji"));
    open_menu.addAction(&spasi);

    QAction reset("&Reset", &container);
    reset.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    reset.setIcon(QIcon(":/slike/novo"));
    reset.setToolTip(QString("Nova ploha za crtanje"));
    open_menu.addAction(&reset);

    QAction close("&Izlaz", &container);
    close.setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
    close.setIcon(QIcon(":/slike/izlaz"));
    close.setToolTip(QString("Izlaz iz aplikacije"));
    open_menu.addAction(&close);

    QMenu edit_menu("&Edit", &container);
    QAction crtaj("&Crtaj", &container);
    crtaj.setCheckable(true);
    crtaj.setIcon(QIcon(":/slike/nacrtaj"));
    crtaj.setToolTip(QString("Nacrtaj duzi"));
    edit_menu.addAction(&crtaj);

    QAction izaberi("&Pretraga", &container);
    izaberi.setCheckable(true);
    izaberi.setIcon(QIcon(":/slike/izaberi"));
    izaberi.setToolTip(QString("Odredi raspon pretrage"));
    edit_menu.addAction(&izaberi);

	QMenu info_menu("&Info", &container);
	QAction info("&Informacije", &container);
	info.setShortcut(QKeySequence(Qt::ALT + Qt::Key_I));
	info.setIcon(QIcon(":/slike/info"));
	info.setToolTip(QString("informacije o aplikaciji"));
	info_menu.addAction(&info);

	QAction pomoc("&Pomoc", &container);
	pomoc.setShortcut(QKeySequence(Qt::ALT + Qt::Key_I));
	pomoc.setIcon(QIcon(":/slike/pomoc"));
	pomoc.setToolTip(QString("Pomoc"));
	info_menu.addAction(&pomoc);

    menu_bar.addMenu(&open_menu);
    menu_bar.addMenu(&edit_menu);
	menu_bar.addMenu(&info_menu);

    QToolBar fileToolBar("&File", &container);
    fileToolBar.addAction(&reset);
    fileToolBar.addAction(&otvori);
    fileToolBar.addAction(&spasi);
    fileToolBar.setGeometry(0, menu_bar.geometry().height() - 10, fileToolBar.geometry().width(), fileToolBar.geometry().height());

    QToolBar  editToolBar("&Edit", &container);
    editToolBar.addAction(&crtaj);
    editToolBar.addAction(&izaberi);
	editToolBar.addSeparator();
    editToolBar.setGeometry(fileToolBar.geometry().width(), menu_bar.geometry().height() - 10, 105, editToolBar.geometry().height());

	QToolBar  infoToolBar("&Info", &container);
	editToolBar.addAction(&info);
	editToolBar.addAction(&pomoc);
	editToolBar.addSeparator();
	editToolBar.addAction(&close);
	editToolBar.setGeometry(editToolBar.geometry().width(), menu_bar.geometry().height() - 10, 180, infoToolBar.geometry().height());

    PretragaProstora aplikacija(&container);
    aplikacija.setGeometry(0, menu_bar.geometry().height() + fileToolBar.geometry().height() - 10, 800, 578);
    container.show();

    QObject::connect(&close, SIGNAL(triggered()), &container, SLOT(close()));
    QObject::connect(&crtaj, SIGNAL(triggered()),   &aplikacija, SLOT(idiUStanjeCrtanja()));
    QObject::connect(&izaberi, SIGNAL(triggered()), &aplikacija, SLOT(idiUStanjePretrage()));    
    QObject::connect(&reset, SIGNAL(triggered()),   &aplikacija, SLOT(reset()));
    QObject::connect(&otvori, SIGNAL(triggered()),  &aplikacija, SLOT(otvoriFajl()));
    QObject::connect(&spasi, SIGNAL(triggered()),   &aplikacija, SLOT(spasiFajl()));
	QObject::connect(&info, SIGNAL(triggered()),   &aplikacija, SLOT(informacije()));
	QObject::connect(&pomoc, SIGNAL(triggered()),   &aplikacija, SLOT(pomoc()));

    QObject::connect(&aplikacija, SIGNAL(jelPretraga(bool)), &izaberi,SLOT(setChecked(bool)));
    QObject::connect(&aplikacija, SIGNAL(jelCrtanje(bool)),  &crtaj,SLOT(setChecked(bool)));

    aplikacija.setFocus();
    return a.exec();*/
//}
