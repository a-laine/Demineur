#include "mainwindow.hpp"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include <mutex>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	a.setApplicationName("Démineur");
	a.setOrganizationName("LAINÉ Aurélien");
	a.setWindowIcon(QIcon(":/images/mine.png"));

	QString locale = QLocale::system().name().section('_', 0, 0);
	QTranslator translator;
	translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&translator);

	MainWindow w;
	w.show();

	std::mutex myMutex;

	return a.exec();
}
